const App = {
  initialized: false,

  async init() {
    if (this.initialized) return;
    this.initialized = true;

    Router.init();

    await this.loadPageContent();

    this.setupNavigation();
    this.setupKeyboardNavigation();

    DevMode.init();

    await this.loadWallpaper();

    Router.goTo('welcome');
    Router.updateUI();
  },

  async loadWallpaper() {
    try {
      const res = await Bridge.getWallpaper();
      if (res && res.data && res.data.length > 0) {
        const el = document.getElementById('wallpaper');
        if (el) {
          el.style.backgroundImage = 'url(' + res.data + ')';
        }
      }
    } catch (e) {
      console.warn('App: Failed to load wallpaper:', e);
    }
  },

  async loadPageContent() {
    const pageModules = [
      'welcome', 'region', 'keyboard', 'keyboard-secondary',
      'network', 'license', 'device-naming', 'account',
      'password', 'privacy', 'customization', 'summary',
      'preparing', 'hi', 'completion'
    ];

    for (const page of pageModules) {
      try {
        const module = window['Page' + this.capitalize(page.replace(/-/g, ''))];
        if (module && module.render) {
          module.render();
        }
      } catch (e) {
        console.error('App: Failed to load page:', page, e);
      }
    }
  },

  capitalize(str) {
    return str.charAt(0).toUpperCase() + str.slice(1);
  },

  setupNavigation() {
    const btnBack = document.getElementById('btn-back');
    const btnNext = document.getElementById('btn-next');

    if (btnBack) {
      btnBack.addEventListener('click', () => {
        const module = this.getCurrentPageModule();
        if (module && module.onBeforeBack) {
          module.onBeforeBack();
        }
        Router.goBackward();
      });
    }

    if (btnNext) {
      btnNext.addEventListener('click', async () => {
        if (Router.isTransitioning) return;

        if (!Router.canProceed()) return;

        const module = this.getCurrentPageModule();
        if (module && module.onBeforeNext) {
          const canProceed = await module.onBeforeNext();
          if (canProceed === false) return;
        }

        if (!Router.canGoNext()) {
          await this.finishSetup();
        } else {
          Router.goForward();
        }
      });
    }
  },

  setupKeyboardNavigation() {
    document.addEventListener('keydown', (e) => {
      if (e.target.tagName === 'INPUT' || e.target.tagName === 'TEXTAREA' || e.target.tagName === 'SELECT') {
        if (e.key === 'Enter') {
          e.preventDefault();
          const btnNext = document.getElementById('btn-next');
          if (btnNext && !btnNext.disabled) btnNext.click();
        }
        return;
      }

      if (e.key === 'Escape') {
        const panel = document.getElementById('dev-panel');
        if (panel && !panel.classList.contains('hidden')) {
          DevMode.deactivate();
          return;
        }
      }
    });
  },

  getCurrentPageModule() {
    const pageId = Router.currentPage;
    const moduleName = 'Page' + this.capitalize(pageId.replace(/-/g, ''));
    return window[moduleName];
  },

  async finishSetup() {
    const preparingPage = document.getElementById('page-preparing');
    const preparingContent = preparingPage.querySelector('.preparing-content');

    Router.goTo('preparing');

    const statusText = preparingContent.querySelector('.preparing-status');
    if (statusText) statusText.textContent = 'Applying your settings...';

    await new Promise(resolve => setTimeout(resolve, 1500));

    if (statusText) statusText.textContent = 'Setting up your device...';
    await new Promise(resolve => setTimeout(resolve, 1500));

    if (statusText) statusText.textContent = 'Almost done...';
    await new Promise(resolve => setTimeout(resolve, 1000));

    try {
      await Bridge.submitAll();
    } catch (e) {
      console.warn('App: Submit warning:', e);
    }

    await new Promise(resolve => setTimeout(resolve, 1000));

    Router.goTo('hi');
    await new Promise(resolve => setTimeout(resolve, 2000));
    Router.goTo('completion');
  }
};

document.addEventListener('DOMContentLoaded', () => {
  App.init();
});

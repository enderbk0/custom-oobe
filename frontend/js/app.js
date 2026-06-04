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

    Router.goTo(PAGE_ORDER[0]);
    Router.updateUI();
  },

  async loadPageContent() {
    const pageModules = [
      PageRegion, PageKeyboard, PageKeyboardSecondary,
      PageNetwork, PageLicense, PageDeviceNaming, PageAccount,
      PagePassword, PagePrivacy, PageCustomization, PageSummary,
      PagePreparing, PageHi, PageCompletion
    ];

    for (const module of pageModules) {
      try {
        if (module && module.render) {
          module.render();
        }
      } catch (e) {
        console.error('App: Failed to load page:', e);
      }
    }
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
    const map = {
      region: PageRegion,
      keyboard: PageKeyboard,
      'keyboard-secondary': PageKeyboardSecondary,
      network: PageNetwork,
      license: PageLicense,
      'device-naming': PageDeviceNaming,
      account: PageAccount,
      password: PagePassword,
      privacy: PagePrivacy,
      customization: PageCustomization,
      summary: PageSummary,
      preparing: PagePreparing,
      hi: PageHi,
      completion: PageCompletion
    };
    return map[Router.currentPage];
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

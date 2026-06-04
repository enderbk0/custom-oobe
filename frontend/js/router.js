const PAGE_ORDER = [
  'welcome',
  'region',
  'keyboard',
  'keyboard-secondary',
  'network',
  'license',
  'device-naming',
  'account',
  'password',
  'privacy',
  'customization',
  'summary',
  'preparing',
  'hi',
  'completion'
];

const PAGE_TITLES = {
  'welcome': 'Welcome',
  'region': 'Region',
  'keyboard': 'Keyboard layout',
  'keyboard-secondary': 'Second keyboard layout',
  'network': 'Let\'s connect you to a network',
  'license': 'License agreement',
  'device-naming': 'Name your device',
  'account': 'Who\'s going to use this device?',
  'password': 'Create a password',
  'privacy': 'Privacy settings for your device',
  'customization': 'Customize your experience',
  'summary': 'Review your settings',
  'preparing': 'Preparing Windows',
  'hi': 'Welcome!',
  'completion': 'You\'re all set!'
};

const Router = {
  currentPage: null,
  history: [],
  isTransitioning: false,
  _pageValidators: {},
  _pageActivateHooks: {},

  init() {
    this.currentPage = PAGE_ORDER[0];
  },

  registerValidator(pageId, validatorFn) {
    this._pageValidators[pageId] = validatorFn;
  },

  registerActivateHook(pageId, hookFn) {
    this._pageActivateHooks[pageId] = hookFn;
  },

  getCurrentIndex() {
    return PAGE_ORDER.indexOf(this.currentPage);
  },

  getPageTitle(pageId) {
    return PAGE_TITLES[pageId] || pageId;
  },

  canGoBack() {
    const idx = this.getCurrentIndex();
    if (idx <= 0) return false;
    const noBackPages = ['preparing', 'hi', 'completion'];
    if (noBackPages.includes(this.currentPage)) return false;
    return true;
  },

  canGoNext() {
    const idx = this.getCurrentIndex();
    if (idx < 0) return false;
    const noNextPages = ['preparing', 'hi', 'completion'];
    if (noNextPages.includes(this.currentPage)) return false;
    return idx < PAGE_ORDER.length - 1;
  },

  goTo(pageId) {
    if (this.isTransitioning) return;
    if (!PAGE_ORDER.includes(pageId)) {
      console.error('Router: Unknown page:', pageId);
      return;
    }
    if (pageId === this.currentPage) return;

    this.isTransitioning = true;
    const fromPage = this.currentPage;
    const toPage = pageId;

    const fromEl = document.getElementById('page-' + fromPage);
    const toEl = document.getElementById('page-' + toPage);
    if (!fromEl || !toEl) {
      this.isTransitioning = false;
      return;
    }

    fromEl.classList.remove('active');
    fromEl.classList.add('exit-left');
    toEl.classList.add('active', 'enter-right');

    setTimeout(() => {
      fromEl.classList.remove('exit-left');
      toEl.classList.remove('enter-right');
      this.currentPage = toPage;
      this.updateUI();

      const hook = this._pageActivateHooks[toPage];
      if (hook) hook();

      this.isTransitioning = false;
    }, 350);
  },

  goForward() {
    const idx = this.getCurrentIndex();
    if (idx < PAGE_ORDER.length - 1) {
      this.history.push(this.currentPage);
      this.goTo(PAGE_ORDER[idx + 1]);
    }
  },

  goBackward() {
    const idx = this.getCurrentIndex();
    if (idx > 0) {
      this.history.pop();
      this.goTo(PAGE_ORDER[idx - 1]);
    }
  },

  updateUI() {
    const idx = this.getCurrentIndex();
    const total = PAGE_ORDER.length;
    const progress = ((idx + 1) / total) * 100;

    const progressBar = document.getElementById('progress-bar');
    const fill = progressBar ? progressBar.querySelector('.progress-fill') : null;
    if (fill) {
      fill.style.width = Math.min(progress, 100) + '%';
    }

    const indicator = document.getElementById('page-indicator');
    if (indicator) {
      indicator.textContent = `${idx + 1} of ${total}`;
    }

    const btnBack = document.getElementById('btn-back');
    if (btnBack) {
      btnBack.disabled = !this.canGoBack();
    }

    const btnNext = document.getElementById('btn-next');
    if (btnNext) {
      const isLast = !this.canGoNext();
      btnNext.textContent = isLast ? 'Finish' : 'Next';
    }
  },

  canProceed() {
    const validator = this._pageValidators[this.currentPage];
    if (validator) {
      return validator();
    }
    return true;
  }
};

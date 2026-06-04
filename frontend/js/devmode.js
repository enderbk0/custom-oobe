const DevMode = {
  isActive: false,

  init() {
    this.setupDevPanel();
    this.setupKeyboardShortcut();
  },

  setupDevPanel() {
    const panel = document.getElementById('dev-panel');
    const closeBtn = document.getElementById('btn-dev-close');
    if (closeBtn) {
      closeBtn.addEventListener('click', () => this.deactivate());
    }

    const select = document.getElementById('dev-page-select');
    if (select) {
      PAGE_ORDER.forEach(page => {
        const option = document.createElement('option');
        option.value = page;
        option.textContent = PAGE_TITLES[page] || page;
        select.appendChild(option);
      });
    }

    const jumpBtn = document.getElementById('btn-dev-jump');
    if (jumpBtn) {
      jumpBtn.addEventListener('click', () => {
        const page = select.value;
        if (page) Router.goTo(page);
      });
    }

    const resetBtn = document.getElementById('btn-dev-reset');
    if (resetBtn) {
      resetBtn.addEventListener('click', () => {
        AppState.reset();
        Router.goTo(PAGE_ORDER[0]);
        this.log('State reset');
      });
    }

    const exportBtn = document.getElementById('btn-dev-export');
    if (exportBtn) {
      exportBtn.addEventListener('click', () => {
        const state = JSON.stringify(AppState.getAll(), null, 2);
        navigator.clipboard.writeText(state).then(() => {
          this.log('State copied to clipboard');
        });
      });
    }

    const importBtn = document.getElementById('btn-dev-import');
    if (importBtn) {
      importBtn.addEventListener('click', () => {
        const input = prompt('Paste state JSON:');
        if (input) {
          try {
            const data = JSON.parse(input);
            AppState.setMultiple(data);
            this.log('State imported');
          } catch (e) {
            this.log('Failed to import: ' + e.message);
          }
        }
      });
    }

    const mockCheckbox = document.getElementById('chk-mock-data');
    if (mockCheckbox) {
      mockCheckbox.addEventListener('change', () => {
        this.log('Mock data: ' + (mockCheckbox.checked ? 'enabled' : 'disabled'));
      });
    }
  },

  setupKeyboardShortcut() {
    document.addEventListener('keydown', (e) => {
      if (e.ctrlKey && e.shiftKey && e.key === 'D') {
        e.preventDefault();
        this.toggle();
      }
    });
  },

  toggle() {
    if (this.isActive) {
      this.deactivate();
    } else {
      this.activate();
    }
  },

  activate() {
    this.isActive = true;
    const panel = document.getElementById('dev-panel');
    if (panel) panel.classList.remove('hidden');
    this.log('Developer mode activated (Ctrl+Shift+D to toggle)');
  },

  deactivate() {
    this.isActive = false;
    const panel = document.getElementById('dev-panel');
    if (panel) panel.classList.add('hidden');
  },

  log(message) {
    const output = document.getElementById('dev-log-output');
    if (output) {
      const timestamp = new Date().toLocaleTimeString();
      output.textContent += `[${timestamp}] ${message}\n`;
      output.scrollTop = output.scrollHeight;
    }
    console.log('[DevMode]', message);
  },

  logState() {
    this.log('Current state: ' + JSON.stringify(AppState.getAll(), null, 2));
  }
};

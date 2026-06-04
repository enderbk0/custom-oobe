const PageSummary = {
  render() {
    const container = document.getElementById('page-summary');
    container.innerHTML = `
      <h1>Review your settings</h1>
      <p class="subtitle">We'll apply these settings when you continue.</p>
      <div class="content-area">
        <div class="summary-section">
          <h3>Region & Language</h3>
          <div class="summary-item">
            <span class="summary-item-label">Region</span>
            <span class="summary-item-value" id="summary-region">-</span>
          </div>
          <div class="summary-item">
            <span class="summary-item-label">Keyboard layout</span>
            <span class="summary-item-value" id="summary-keyboard">-</span>
          </div>
        </div>
        <div class="summary-section">
          <h3>Device</h3>
          <div class="summary-item">
            <span class="summary-item-label">Device name</span>
            <span class="summary-item-value" id="summary-device-name">-</span>
          </div>
        </div>
        <div class="summary-section">
          <h3>Account</h3>
          <div class="summary-item">
            <span class="summary-item-label">Username</span>
            <span class="summary-item-value" id="summary-account">-</span>
          </div>
          <div class="summary-item">
            <span class="summary-item-label">Password</span>
            <span class="summary-item-value" id="summary-password">-</span>
          </div>
        </div>
        <div class="summary-section">
          <h3>Privacy</h3>
          <div class="summary-item">
            <span class="summary-item-label">Location</span>
            <span class="summary-item-value" id="summary-location">-</span>
          </div>
          <div class="summary-item">
            <span class="summary-item-label">Diagnostic data</span>
            <span class="summary-item-value" id="summary-diagnostics">-</span>
          </div>
        </div>
      </div>
    `;

    Router.registerActivateHook('summary', () => this.updateDisplay());
  },

  updateDisplay() {
    const state = AppState.getAll();

    const setText = (id, value) => {
      const el = document.getElementById(id);
      if (el) el.textContent = value || '-';
    };

    setText('summary-region', state.region || '-');
    setText('summary-keyboard', state.keyboardLayout || '-');
    setText('summary-device-name', state.deviceName || '-');
    setText('summary-account', state.accountName || '-');
    setText('summary-password', state.password ? 'Configured' : 'None');

    const privacy = state.privacySettings || {};
    setText('summary-location', privacy.location ? 'On' : 'Off');
    setText('summary-diagnostics', privacy.diagnostics ? 'On' : 'Off');
  },

  onBeforeNext() {
    this.updateDisplay();
    return true;
  }
};

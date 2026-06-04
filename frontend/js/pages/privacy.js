const PagePrivacy = {
  render() {
    const container = document.getElementById('page-privacy');
    container.innerHTML = `
      <h1>Privacy settings for your device</h1>
      <p class="subtitle">Choose the privacy settings that are right for you. You can change these later.</p>
      <div class="content-area">
        <div class="privacy-section">
          <div class="form-toggle">
            <div>
              <div class="toggle-label">Location</div>
              <div class="toggle-description">Let Windows and apps access your location</div>
            </div>
            <input type="checkbox" id="privacy-location" checked>
          </div>
        </div>
        <div class="privacy-section">
          <div class="form-toggle">
            <div>
              <div class="toggle-label">Find my device</div>
              <div class="toggle-description">Help protect your device by letting Windows find it</div>
            </div>
            <input type="checkbox" id="privacy-find-device" checked>
          </div>
        </div>
        <div class="privacy-section">
          <div class="form-toggle">
            <div>
              <div class="toggle-label">Ad tracking</div>
              <div class="toggle-description">Let apps show me personalized ads</div>
            </div>
            <input type="checkbox" id="privacy-ads">
          </div>
        </div>
        <div class="privacy-section">
          <div class="form-toggle">
            <div>
              <div class="toggle-label">Diagnostic data</div>
              <div class="toggle-description">Send diagnostic data to Microsoft</div>
            </div>
            <input type="checkbox" id="privacy-diagnostics" checked>
          </div>
        </div>
        <div class="privacy-section">
          <div class="form-toggle">
            <div>
              <div class="toggle-label">Tailored experiences</div>
              <div class="toggle-description">Get tips and suggestions based on your usage</div>
            </div>
            <input type="checkbox" id="privacy-tailored">
          </div>
        </div>
      </div>
    `;
  },

  onBeforeNext() {
    const settings = {
      location: document.getElementById('privacy-location').checked,
      findDevice: document.getElementById('privacy-find-device').checked,
      ads: document.getElementById('privacy-ads').checked,
      diagnostics: document.getElementById('privacy-diagnostics').checked,
      tailored: document.getElementById('privacy-tailored').checked
    };
    AppState.set('privacySettings', settings);
    Bridge.setPrivacySettings(settings);
    return true;
  }
};

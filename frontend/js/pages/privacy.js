const PagePrivacy = {
  render() {
    const container = document.getElementById('page-privacy');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Privacy settings for your device</h1>
      </div>
      <p class="content-lead">Choose the privacy settings that are right for you. You can change these later.</p>
      <fieldset>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="privacy-location" checked />
          <label for="privacy-location">
            <div style="color:#fff;font-weight:600">Location</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Let Windows and apps access your location</p>
          </label>
        </div>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="privacy-find-device" checked />
          <label for="privacy-find-device">
            <div style="color:#fff;font-weight:600">Find my device</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Help protect your device by letting Windows find it</p>
          </label>
        </div>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="privacy-ads" />
          <label for="privacy-ads">
            <div style="color:#fff;font-weight:600">Ad tracking</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Let apps show me personalized ads</p>
          </label>
        </div>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="privacy-diagnostics" checked />
          <label for="privacy-diagnostics">
            <div style="color:#fff;font-weight:600">Diagnostic data</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Send diagnostic data to Microsoft</p>
          </label>
        </div>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="privacy-tailored" />
          <label for="privacy-tailored">
            <div style="color:#fff;font-weight:600">Tailored experiences</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Get tips and suggestions based on your usage</p>
          </label>
        </div>
      </fieldset>
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

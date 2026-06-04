const PageLicense = {
  render() {
    const container = document.getElementById('page-license');
    container.innerHTML = `
      <h1>License Agreement</h1>
      <p class="subtitle">Please review and accept the license terms.</p>
      <div class="content-area">
        <div class="license-container">
          <p><strong>MICROSOFT SOFTWARE LICENSE TERMS</strong></p>
          <br>
          <p>THIS IS A SAMPLE LICENSE AGREEMENT FOR DEMONSTRATION PURPOSES.</p>
          <br>
          <p>This is not the real Microsoft EULA. This is placeholder text for the custom OOBE prototype.</p>
          <br>
          <p>In a production deployment, the actual Windows license terms would be displayed here as provided by Microsoft.</p>
          <br>
          <p><strong>1. Scope of License.</strong> The software is licensed, not sold. This agreement only gives you some rights to use the software included with your device.</p>
          <br>
          <p><strong>2. Installation and Use.</strong> You may install and use one copy of the software on one device.</p>
          <br>
          <p><strong>3. Privacy.</strong> Your privacy is important to us. Please review the Microsoft Privacy Statement.</p>
          <br>
          <p><strong>4. Updates.</strong> The software may periodically check for updates and download and install them for you.</p>
          <br>
          <p><strong>5. Support.</strong> Contact the manufacturer for support options.</p>
        </div>
        <div class="form-checkbox">
          <input type="checkbox" id="license-accept">
          <label for="license-accept" class="checkbox-label">I accept the license terms</label>
        </div>
      </div>
    `;

    const checkbox = document.getElementById('license-accept');
    checkbox.addEventListener('change', () => {
      if (checkbox.checked) {
        AppState.set('licenseAccepted', true);
        Bridge.acceptLicense();
      } else {
        AppState.set('licenseAccepted', false);
      }
    });
  },

  onBeforeNext() {
    const accepted = AppState.get('licenseAccepted');
    if (!accepted) {
      return false;
    }
    return true;
  }
};

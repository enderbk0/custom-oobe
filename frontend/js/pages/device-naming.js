const PageDeviceNaming = {
  render() {
    const container = document.getElementById('page-device-naming');
    container.innerHTML = `
      <h1>Name your device</h1>
      <p class="subtitle">Give your device a name. You'll see this name on the network.</p>
      <div class="content-area">
        <div class="form-group">
          <label class="form-label" for="device-name">Device name</label>
          <input type="text" id="device-name" class="form-input"
                 placeholder="e.g. DESKTOP-ABC123" maxlength="15"
                 autocomplete="off" spellcheck="false">
        </div>
        <p class="subtitle" style="font-size:var(--font-size-sm)">
          You can change it later in Settings &gt; System &gt; About.
        </p>
      </div>
    `;

    const input = document.getElementById('device-name');
    input.addEventListener('input', () => {
      ValidationUI.clearError('device-name');
    });

    input.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    const saved = AppState.get('deviceName');
    if (saved) input.value = saved;

    Router.registerValidator('device-naming', () => {
      const value = input.value.trim();
      const error = Validators.deviceName(value);
      if (error) {
        ValidationUI.showError('device-name', error);
        return false;
      }
      AppState.set('deviceName', value);
      Bridge.setDeviceName(value);
      return true;
    });
  }
};

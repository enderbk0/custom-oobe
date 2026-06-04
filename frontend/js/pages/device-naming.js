const PageDeviceNaming = {
  render() {
    const container = document.getElementById('page-device-naming');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Name your device</h1>
      </div>
      <p class="content-lead">Give your device a name so you can recognize it on the network.</p>
      <fieldset>
        <legend>Device name</legend>
        <div class="template-input">
          <input type="text" id="device-name" class="win-textbox win-textbox-PuaCompatible-font"
                 placeholder="e.g. DESKTOP-ABC123" maxlength="15" autofocus />
        </div>
      </fieldset>
    `;

    const input = document.getElementById('device-name');
    input.addEventListener('input', () => {
      AppState.set('deviceName', input.value);
      Bridge.setDeviceName(input.value);
    });
    input.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    const saved = AppState.get('deviceName');
    if (saved) input.value = saved;
  },

  onBeforeNext() {
    const name = AppState.get('deviceName');
    if (!name || name.trim().length === 0) return false;
    return true;
  }
};

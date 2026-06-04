const PageCustomization = {
  render() {
    const container = document.getElementById('page-customization');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Customize your experience</h1>
      </div>
      <p class="content-lead">Choose how you want to use this device.</p>
      <fieldset>
        <div class="template-input inputType_radio">
          <input type="radio" name="customization" id="cust-personal" value="personal" checked />
          <label for="cust-personal">
            <div style="color:#fff;font-weight:600">Personal use</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Set up for personal use with your Microsoft account</p>
          </label>
        </div>
        <div class="template-input inputType_radio">
          <input type="radio" name="customization" id="cust-work" value="work" />
          <label for="cust-work">
            <div style="color:#fff;font-weight:600">Work or school</div>
            <p style="margin:0;color:rgba(255,255,255,0.7);font-size:12px">Set up for work or school with an organizational account</p>
          </label>
        </div>
      </fieldset>
    `;

    const radios = container.querySelectorAll('input[name="customization"]');
    radios.forEach(r => {
      r.addEventListener('change', () => {
        if (r.checked) {
          AppState.set('customization', r.value);
        }
      });
    });

    const saved = AppState.get('customization');
    if (saved) {
      const el = container.querySelector(`#cust-${saved}`);
      if (el) el.checked = true;
    }
  }
};

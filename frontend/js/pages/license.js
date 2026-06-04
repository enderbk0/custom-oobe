const PageLicense = {
  render() {
    const container = document.getElementById('page-license');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">License agreement</h1>
      </div>
      <p class="content-lead">Please review the license agreement that applies to this device.</p>
      <div class="eula-iframe">
        <div class="scroll-view" style="max-height:280px; border:1px solid rgba(255,255,255,0.2); padding:16px; box-sizing:border-box">
          <p style="color:#fff">MICROSOFT SOFTWARE LICENSE TERMS</p>
          <p style="color:rgba(255,255,255,0.7); font-size:12px">
            These license terms are an agreement between Microsoft Corporation (or based on where you live, one of its affiliates) and you. Please read them. They apply to the software named above, which includes the media on which you received it, if any. The terms also apply to any Microsoft
            <br><br>
            updates, supplements, Internet-based services, and support services for this software, unless other terms accompany those items. If so, those terms apply.
            <br><br>
            BY USING THE SOFTWARE, YOU ACCEPT THESE TERMS. IF YOU DO NOT ACCEPT THEM, DO NOT USE THE SOFTWARE. INSTEAD, RETURN IT TO THE RETAILER FOR A REFUND OR CREDIT. If you cannot obtain a refund there, contact Microsoft or the Microsoft affiliate serving your country for information about Microsoft's refund policies. See www.microsoft.com/refund.
            <br><br>
            As described below, using the software also operates as your consent to the transmission of certain computer information during activation, validation, and during the use of Internet-based services.
            <br><br>
            If you comply with these license terms, you have the rights below.
          </p>
        </div>
      </div>
      <fieldset>
        <div class="template-input inputType_checkbox">
          <input type="checkbox" id="license-accept" />
          <label for="license-accept">I accept the license terms</label>
        </div>
      </fieldset>
    `;

    const cb = document.getElementById('license-accept');
    cb.addEventListener('change', () => {
      AppState.set('licenseAccepted', cb.checked);
    });
  },

  onBeforeNext() {
    const accepted = AppState.get('licenseAccepted');
    if (!accepted) return false;
    return true;
  }
};

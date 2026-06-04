const PagePassword = {
  render() {
    const container = document.getElementById('page-password');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Create a password</h1>
      </div>
      <p class="content-lead">Protect your account with a strong password.</p>
      <fieldset aria-labelledby="PasswordLegend">
        <legend id="PasswordLegend">Password</legend>
        <div class="template-input" id="password_input">
          <input type="password" id="password" class="win-textbox win-textbox-PuaCompatible-font"
                 placeholder="Enter a password" maxlength="127" autocomplete="new-password" autofocus />
        </div>
      </fieldset>
      <fieldset aria-labelledby="PasswordConfirmLegend">
        <legend id="PasswordConfirmLegend">Re-enter password</legend>
        <div class="template-input" id="password_confirm_input">
          <input type="password" id="password-confirm" class="win-textbox win-textbox-PuaCompatible-font"
                 placeholder="Re-enter your password" maxlength="127" autocomplete="new-password" />
        </div>
      </fieldset>
      <fieldset aria-labelledby="PasswordHintLegend">
        <legend id="PasswordHintLegend">Password hint</legend>
        <div class="template-input" id="password_hint_input">
          <input type="text" id="password-hint" class="win-textbox win-textbox-PuaCompatible-font"
                 placeholder="Enter a hint" maxlength="127" autocomplete="off" />
        </div>
      </fieldset>
    `;

    const password = document.getElementById('password');
    const confirm = document.getElementById('password-confirm');
    const hint = document.getElementById('password-hint');

    password.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') { e.preventDefault(); confirm.focus(); }
    });
    confirm.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') { e.preventDefault(); hint.focus(); }
    });
    hint.addEventListener('keydown', (e) => {
      if (e.key === 'Tab' && !e.shiftKey) {
        e.preventDefault();
        document.getElementById('btn-next').focus();
      }
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    Router.registerValidator('password', () => {
      const pwd = password.value;
      const conf = confirm.value;
      const pwdError = Validators.password(pwd);
      if (pwdError) return false;
      if (pwd !== conf) return false;
      AppState.set('password', '********');
      Bridge.setPassword();
      return true;
    });
  }
};

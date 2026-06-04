const PagePassword = {
  render() {
    const container = document.getElementById('page-password');
    container.innerHTML = `
      <h1>Create a password</h1>
      <p class="subtitle">Protect your account with a strong password.</p>
      <div class="content-area">
        <div class="form-group">
          <label class="form-label" for="password">Password</label>
          <input type="password" id="password" class="form-input"
                 placeholder="Enter a password" maxlength="127"
                 autocomplete="new-password">
        </div>
        <div class="form-group">
          <label class="form-label" for="password-confirm">Re-enter password</label>
          <input type="password" id="password-confirm" class="form-input"
                 placeholder="Re-enter your password" maxlength="127"
                 autocomplete="new-password">
        </div>
        <div class="form-group">
          <label class="form-label" for="password-hint">Password hint</label>
          <input type="text" id="password-hint" class="form-input"
                 placeholder="Enter a hint" maxlength="127"
                 autocomplete="off">
        </div>
      </div>
    `;

    const password = document.getElementById('password');
    const confirm = document.getElementById('password-confirm');
    const hint = document.getElementById('password-hint');

    [password, confirm, hint].forEach(el => {
      el.addEventListener('input', () => {
        ValidationUI.clearError(el.id);
      });
    });

    password.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        confirm.focus();
      }
    });

    confirm.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    hint.addEventListener('keydown', (e) => {
      if (e.key === 'Tab' && !e.shiftKey) {
        e.preventDefault();
        document.getElementById('btn-next').focus();
      }
    });

    Router.registerValidator('password', () => {
      const pwd = password.value;
      const conf = confirm.value;

      const pwdError = Validators.password(pwd);
      if (pwdError) {
        ValidationUI.showError('password', pwdError);
        return false;
      }

      if (pwd !== conf) {
        ValidationUI.showError('password-confirm', 'Passwords do not match');
        return false;
      }

      AppState.set('password', '********');
      Bridge.setPassword();
      return true;
    });
  }
};

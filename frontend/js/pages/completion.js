const PageCompletion = {
  render() {
    const container = document.getElementById('page-completion');
    container.innerHTML = `
      <div class="animate-fade-in-scale">
        <div class="completion-icon">
          <svg viewBox="0 0 80 80" fill="none" xmlns="http://www.w3.org/2000/svg">
            <circle cx="40" cy="40" r="32" stroke="currentColor" stroke-width="2" fill="none" opacity="0.2"/>
            <path d="M28 42L36 50L52 32" stroke="currentColor" stroke-width="3.5"
                  stroke-linecap="round" stroke-linejoin="round"
                  stroke-dasharray="48" stroke-dashoffset="48"
                  style="animation: checkmark 0.6s ease-out 0.3s forwards"/>
          </svg>
        </div>
        <h1 class="animate-fade-in delay-200">You're all set!</h1>
        <p class="animate-fade-in delay-300">Your device is ready to go. We hope you enjoy it!</p>
        <div class="action-area animate-fade-in delay-500">
          <p class="subtitle">This completes the custom OOBE demonstration.</p>
          <p class="subtitle" style="font-size:var(--font-size-sm); margin-top: var(--spacing-md);">
            This is a Phase 1 prototype. No changes have been made to your system.
          </p>
        </div>
      </div>
    `;
  }
};

const PageWelcome = {
  render() {
    const container = document.getElementById('page-welcome');
    container.innerHTML = `
      <div class="welcome-icon">
        <svg viewBox="0 0 72 72" fill="none" xmlns="http://www.w3.org/2000/svg">
          <rect x="8" y="8" width="56" height="56" rx="12" fill="currentColor" opacity="0.1"/>
          <path d="M36 20L52 36L36 52" stroke="currentColor" stroke-width="2.5"
                stroke-linecap="round" stroke-linejoin="round"/>
          <path d="M20 36H48" stroke="currentColor" stroke-width="2.5"
                stroke-linecap="round" stroke-linejoin="round"/>
        </svg>
      </div>
      <h1>Welcome!</h1>
      <p>Let's get you started with your new device. We'll walk you through a few steps to set everything up.</p>
      <div class="action-area">
        <button class="btn btn-primary btn-next-inline" style="width:100%">
          Get started
          <svg width="16" height="16" viewBox="0 0 16 16" fill="none">
            <path d="M6 4L10 8L6 12" stroke="currentColor" stroke-width="1.5"
                  stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </button>
      </div>
    `;

    container.querySelector('.btn-next-inline').addEventListener('click', () => {
      Router.goForward();
    });
  }
};

const PageHi = {
  render() {
    const container = document.getElementById('page-hi');
    const userName = AppState.get('accountName', 'there');
    container.innerHTML = `
      <div class="hi-content animate-fade-in-scale">
        <div class="welcome-icon">
          <svg viewBox="0 0 72 72" fill="none" xmlns="http://www.w3.org/2000/svg">
            <circle cx="36" cy="36" r="28" stroke="currentColor" stroke-width="2" fill="none"/>
            <path d="M28 36L34 42L44 30" stroke="currentColor" stroke-width="3"
                  stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </div>
        <h1>Hi, ${userName}!</h1>
        <p class="animate-fade-in delay-200">We're setting things up just for you.</p>
      </div>
    `;
  }
};

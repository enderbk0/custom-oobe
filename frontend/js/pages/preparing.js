const PagePreparing = {
  render() {
    const container = document.getElementById('page-preparing');
    container.innerHTML = `
      <div class="preparing-content">
        <div class="preparing-spinner"></div>
        <h1>Preparing Windows</h1>
        <p class="preparing-status">Just a moment...</p>
      </div>
    `;
  }
};

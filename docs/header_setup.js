/**
 * header_setup.js
 * Initializes Doxygen Awesome extensions
 * (Must use Vanilla JS because Doxygen does not include jQuery)
 */
window.addEventListener("DOMContentLoaded", () => {
  // 1. Initialize Dark Mode Toggle
  if (typeof DoxygenAwesomeDarkModeToggle !== "undefined") {
    DoxygenAwesomeDarkModeToggle.init();
  }

  // 2. Initialize Copy to Clipboard Button
  if (typeof DoxygenAwesomeFragmentCopyButton !== "undefined") {
    DoxygenAwesomeFragmentCopyButton.init();
  }

  // 3. Initialize Paragraph Links
  if (typeof DoxygenAwesomeParagraphLink !== "undefined") {
    DoxygenAwesomeParagraphLink.init();
  }

  // 4. Initialize Interactive Table of Contents
  if (typeof DoxygenAwesomeInteractiveToc !== "undefined") {
    DoxygenAwesomeInteractiveToc.init();
  }
});

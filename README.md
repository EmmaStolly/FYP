# PawPoint

A project conceived, designed, and developed by Emma Stolliday, Jasmine Price, and Kev White as a submission for a final stage undergraduate module in Internet Design.

## Installation (for development)

- Download or clone this repository
- Open a terminal/command line window and navigate to the project directory
- From the project directory (where `app.js` is located), enter `npm i` to install dependencies
- Run `node app`
- Navigate to `localhost:3000` in your preferred browser

### Considerations when downloading for development

- Create a .gitignore file in the app directory and add the following entries to ensure unnecessary files are ignored when uploading to GitHub:
  - `/node_modules`
  - `/package-lock.json`
- Create a remote repo to update from
  - Command line: `git remote add origin http://github.com/krelith/pawpoint.git`
- Before adding files/changes to your remote, ensure the aforementioned .gitignore file is saved
- To push changes:
  - `git add [CHANGED_FILE]` where `[CHANGED_FILE]` is the file you've changed plus its extension, e.g. `views/main.hbs`
    - N.B. Do not add using the catch-all (asterisk) since it could overwrite changes being implemented on a separate branch
  - `git commit -m '[MSG]'` where `[MSG]` is the message indicating what you have updated (keep it simple)
  - `git push origin master` to push your changes to the master repository on GitHub

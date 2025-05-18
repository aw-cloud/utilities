# A Collection of Useful Scripts and Tools for the Command Line

## Sanitise Filename

For keeping files consistently named.
- Replaces special characters, non-printing characters, whitespace etc. and produces a Capitalised_Snake_Case filename.
- Some replacements are designed to show what was replaced, e.g. `&` becomes `_and_` for readability.
- Others just remove characters altogether, e.g. `filename_.ext` becomes `filename.ext`.
- Extensions and the dirname are not modified so you can safely do `mv -u file $(sanitise_filename file)` for renaming.
- This is a port from an original perl script and is currently a minimal working example.


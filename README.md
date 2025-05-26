# A Collection of Useful Scripts and Tools for the Command Line

## Sanitise Filename

For keeping files consistently named.
- Replaces special characters, non-printing characters, whitespace etc. and produces a Capitalised_Snake_Case filename.
- Some replacements are designed to show what was replaced, e.g. `&` becomes `_and_` for readability.
- Others just remove characters altogether, e.g. `filename_.ext` becomes `filename.ext`.
- Extensions and the dirname are not modified (by default) so you can safely do `mv -u file $(sanitise_filename file)` for renaming.
- Relies on the boost regex library and makes use of modern C++23 features

## Mvsan

A bash script that renames your files and folders automatically using the sanitise_filename program to do the string manipulation.
- Pass -k|--keepcase to preserve the original case of your filenames.
- The script calls sanitise_filename with -n (no extension) for directories so that `mvsan *` etc. work as expected.
- The `mv` command used internally is called with -n so files won't be overwritten if a sanitised filename conflicts with an existing file.

# CAD Practicals — GitHub Pages site

This folder is a ready-to-publish static website containing the 11 source files from the uploaded practicals archive. The original source filenames and contents are preserved. The temporary editor backup `practical_05_add_sub.asm~` is intentionally excluded.

## Publish with GitHub Pages

1. Sign in to GitHub and create a **new public repository**, for example `cad-practicals`.
2. Extract this ZIP on your computer.
3. Open the new repository and choose **Add file → Upload files**.
4. Upload the *contents* of this folder (the `index.html`, `assets` folder, `practicals` folder and `README.md`) to the repository's top level. Do not upload only the ZIP file.
5. Commit the upload to the `main` branch.
6. Open **Settings → Pages** in the repository.
7. Under **Build and deployment**, choose **Deploy from a branch**.
8. Select branch **main** and folder **/(root)**, then click **Save**.
9. After GitHub finishes publishing, the site will be available at:

   `https://YOUR-GITHUB-USERNAME.github.io/cad-practicals/`

   Replace `YOUR-GITHUB-USERNAME` with your GitHub username. If you chose a different repository name, use that name in the URL.

## Included practicals

- 01–03: C programs
- 05–08: 8085 assembly
- 09–10: CUDA
- 11–12: OpenCL

Practical 04 was not present in the uploaded archive, so the original numbering is retained and no file has been invented for it.

## Notes

- The site uses relative file paths and is designed for GitHub Pages project sites.
- Code is loaded as text for viewing and copying; it is not compiled or executed by the site.
- To update a practical later, replace the corresponding file in `practicals/` and commit the change.

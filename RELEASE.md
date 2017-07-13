# How to release a new version (note to self)

1. Update version field in `leg.yml`
2. Update `CHANGELOG.md`
3. Run `leg doc -z`
4. Commit and push
5. Run `leg deploy`
6. Create new release on GitHub, uploading `doc/kilo-tutorial-{version}.zip`
   and copying notes from the `CHANGELOG.md`


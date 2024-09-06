### What are the requirements for a library to be added to Library Manager?

- [?] The library must be fully compliant with the [Arduino Library Specification](https://arduino.github.io/arduino-cli/latest/library-specification).
- [X] The library must have [a library.properties file](https://arduino.github.io/arduino-cli/latest/library-specification/#library-metadata), in compliance with the Arduino Library 1.5 format.
- [X] The library.properties file must be located in the root of the repository.
- [X] The library must not have the same library.properties `name` value (regardless of case) as another library previously added to the Library Manager list.
- [X] For 3rd party libraries, the `name` field in library.properties must not start with `Arduino`.
- [X] The library repository must not contain any `.exe` files.
- [X] The library repository must not contain a [`.development` file](https://arduino.github.io/arduino-cli/latest/library-specification/#development-flag-file).
- [X] The library repository must not contain any [symlinks](https://wikipedia.org/wiki/Symbolic_link).
- [X] The library repository must not contain any files detected as infected by our antivirus scan.
- [ ] The library repository must have a [Git tag](https://git-scm.com/book/en/v2/Git-Basics-Tagging) (or [release](https://docs.github.com/repositories/releasing-projects-on-github/managing-releases-in-a-repository)) and must have been compliant with all the above requirements at the time of that tag.
- [ ] The library repository must be hosted on a major Git-hosting website like GitHub, BitBucket or GitLab (other hosting sites may be considered on request).

Arduino has created a command line tool to check libraries for compliance with all the Library Manager requirements:

https://github.com/arduino/arduino-lint

Arduino Lint is also available as a GitHub Actions action that can be used in the CI systems of Arduino library repositories to ensure continued compliance:

https://github.com/arduino/arduino-lint-action
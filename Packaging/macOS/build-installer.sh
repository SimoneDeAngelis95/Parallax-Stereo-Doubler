#!/bin/zsh

set -euo pipefail

scriptDirectory="${0:A:h}"
projectDirectory="${scriptDirectory:h:h}"
buildDirectory="${1:-${projectDirectory}/build-xcode}"
configuration="${2:-Release}"

version="$(awk '/^[[:space:]]*VERSION[[:space:]]+/ { print $2; exit }' \
    "${projectDirectory}/CMakeLists.txt")"

if [[ -z "${version}" ]]; then
    print -u2 "Could not read the project version from CMakeLists.txt."
    exit 1
fi

auSource="${buildDirectory}/Parallax_artefacts/${configuration}/AU/Parallax.component"
vst3Source="${buildDirectory}/Parallax_artefacts/${configuration}/VST3/Parallax.vst3"

for pluginBundle in "${auSource}" "${vst3Source}"; do
    if [[ ! -d "${pluginBundle}" ]]; then
        print -u2 "Missing plugin bundle: ${pluginBundle}"
        print -u2 "Build Parallax in ${configuration} mode before creating the installer."
        exit 1
    fi
done

temporaryDirectory="$(mktemp -d "${TMPDIR:-/tmp}/parallax-installer.XXXXXX")"
trap 'rm -rf "${temporaryDirectory}"' EXIT

auPayloadDirectory="${temporaryDirectory}/au-payload"
vst3PayloadDirectory="${temporaryDirectory}/vst3-payload"
componentPackagesDirectory="${temporaryDirectory}/packages"
distributionFile="${temporaryDirectory}/Distribution.xml"
outputDirectory="${scriptDirectory}/output"
outputPackage="${outputDirectory}/Parallax-${version}-macOS.pkg"
temporaryOutputPackage="${temporaryDirectory}/Parallax-${version}-macOS.pkg"

mkdir -p \
    "${auPayloadDirectory}/Library/Audio/Plug-Ins/Components" \
    "${vst3PayloadDirectory}/Library/Audio/Plug-Ins/VST3" \
    "${componentPackagesDirectory}" \
    "${outputDirectory}"

ditto "${auSource}" \
    "${auPayloadDirectory}/Library/Audio/Plug-Ins/Components/Parallax.component"

ditto "${vst3Source}" \
    "${vst3PayloadDirectory}/Library/Audio/Plug-Ins/VST3/Parallax.vst3"

buildComponentPackage()
{
    local payloadDirectory="$1"
    local scriptsDirectory="$2"
    local identifier="$3"
    local packagePath="$4"
    local componentPropertyList="${temporaryDirectory}/${identifier}.plist"

    pkgbuild --analyze --root "${payloadDirectory}" "${componentPropertyList}"
    /usr/libexec/PlistBuddy \
        -c "Add :0:BundleIsRelocatable bool false" \
        "${componentPropertyList}"

    pkgbuild \
        --root "${payloadDirectory}" \
        --scripts "${scriptsDirectory}" \
        --component-plist "${componentPropertyList}" \
        --identifier "${identifier}" \
        --version "${version}" \
        --install-location "/" \
        --ownership recommended \
        "${packagePath}"
}

buildComponentPackage \
    "${auPayloadDirectory}" \
    "${scriptDirectory}/scripts/au" \
    "com.simonedeangelis.parallax.au" \
    "${componentPackagesDirectory}/Parallax-AU.pkg"

buildComponentPackage \
    "${vst3PayloadDirectory}" \
    "${scriptDirectory}/scripts/vst3" \
    "com.simonedeangelis.parallax.vst3" \
    "${componentPackagesDirectory}/Parallax-VST3.pkg"

sed "s/@VERSION@/${version}/g" \
    "${scriptDirectory}/Distribution.xml.in" > "${distributionFile}"

productbuild \
    --distribution "${distributionFile}" \
    --package-path "${componentPackagesDirectory}" \
    "${temporaryOutputPackage}"

mv -f "${temporaryOutputPackage}" "${outputPackage}"

print "Installer created:"
print "${outputPackage}"

#!/bin/bash

validate_version_format() {
    local version_file_content=$(cat VERSION)
    
    if [[ ! "$version_file_content" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        echo "- VERSION file format... FAIL ($version_file_content)."
        return 1
    else
        echo "- VERSION file format... OK"
        return 0
    fi
}

check_tag_version_match() {
    # Get the current commit's tag (if it exists)
    local current_commit_tag=$(git describe --exact-match --tags HEAD 2>/dev/null | sed 's/^v//')
    local version_file=$(cat VERSION)

    if [[ -n "$current_commit_tag" ]]; then
        # Current commit is tagged
        if [[ "$current_commit_tag" == "$version_file" ]]; then
            echo "- Git tag consistent with VERSION file... OK"
            return 0
        else
            echo "- Git tag consistent with VERSION file... FAIL"
            echo "ERROR: Tag ($current_commit_tag) does not match VERSION file content ($version_file)."
            return 1
        fi
    else
        # Current commit is not tagged, so VERSION file must have been increased manually
        # Lets check...

        local last_tag=$(git describe --tags --abbrev=0 | sed 's/^v//')

        local smallest_tag=$(printf '%s\n' "$version_file" "$last_tag" | sort -V | head -n1)

        if [[ "$last_tag" == "$version_file" || "$smallest_tag" == "$version_file" ]]; then
            echo "- VERSION file bumped after release... FAIL"
            echo "ERROR: VERSION file content ($version_file) should be greater than the last tag ($last_tag), did you forget to bump it?"
            return 1
        else
            echo "- VERSION file bumped after release... OK"
            echo ""
            echo "Last tag ($last_tag), version in development ($version_file)".
            return 0
        fi
    fi
}

# Function to check if Cargo.toml version matches the VERSION file
check_cargo_version_match() {
    local version_file=$(cat VERSION)
    # Extract the version line from Cargo.toml
    local cargo_version=$(grep '^version = ' Cargo.toml | sed 's/version = "\(.*\)"/\1/')

    if [[ "$cargo_version" == "$version_file" ]]; then
        echo "- Cargo.toml consistent with VERSION file... OK"
        return 0
    else
        echo "Error: Cargo.toml version ($cargo_version) does not match VERSION file content ($version_file)."
        return 1
    fi
}

validate_version_format || exit 1
check_cargo_version_match || exit 1
check_tag_version_match || exit 1


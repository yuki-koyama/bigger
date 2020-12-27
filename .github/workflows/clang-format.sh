clang-format src/*.cpp include/**/*.hpp examples/**/*.*pp --output-replacements-xml | grep -c "<replacement " >/dev/null
if [ $? -ne 1 ]; then echo "Ill-styled code detected" && exit 1; fi

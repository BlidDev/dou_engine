
premake=./utils/premake5

$premake $2 ${1:-gmake} 

echo "Would you like to generate a compile_commands.json file?"
select yn in "Yes" "Generate in build directory" "No"; do
    case $yn in
        Yes ) $premake ecc; break;;
        "Generate in build directory" ) $premake ecc; mv compile_commands.json build/; break;;
        No ) exit;;
    esac
done

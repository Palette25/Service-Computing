# Print Welcome messages and read input
echo "----------------- Make Package For Go -----------------"
read -p "Please input package name: " name

# Begin mkdir and move file
mkdir -p ../src/${name}
mv ./${name}.go ../src/${name}/

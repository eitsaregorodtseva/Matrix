input="in"
rm -rf out

while IFS= read -r line
do
    ./main "$line" >> "out"
done < "$input"


DIFF=$(comm -2 -3 control_out out) 
if [ "$DIFF" != "" ] 
then
    echo "Errors!"
    echo "$DIFF"
else 
    echo "No errors"
fi
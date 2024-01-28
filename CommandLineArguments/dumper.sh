./arger-yuyi -cap "help is on the way"

status=$?


if [ $status -eq 0 ]; then
    echo "Program executed successfully."
else
    echo "Program failed with error code: $status"
fi

./arger-yuyi -a "help is on the way"

status=$?

if [ $status -eq 0 ]; then
    echo "Program executed successfully."
else
    echo "Program failed with error code: $status"
fi

./arger-yuyi -u

status=$?

if [ $status -eq 0 ]; then
    echo "Program executed successfully."
else
    echo "Program failed with error code: $status"
fi



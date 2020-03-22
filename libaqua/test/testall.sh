
# build/serversockettest 12345
build/sockettest localhost 12345

case $? in
  0 ) echo "Okay" ;;
  * ) echo "Failed." ;;
esac

exit

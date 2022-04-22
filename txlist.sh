#check
./clientA Luke
./clientA Ali
./clientB Luke1
./clientB Ali1
#transfer
./clientA Luke Ali 10
./clientA Ali Luke 10
./clientB Luke Ali 10
./clientB Ali Luke 10
./clientA Luke Ali 100000000
./clientB Luke Ali 100000000
./clientA Luke1 Ali 10
./clientB Luke Ali1 10
./clientA Luke1 Ali1 10
./clientB Luke1 Ali 100000000
./clientA Luke Ali1 100000000
./clientB Luke1 Ali1 100000000
#TXLIST
./clientA TXLIST
./clientB TXLIST
#stats
./clientA Luke stats
./clientB Luke stats
./clientA Luke1
./clientB Luke1
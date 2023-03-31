kill -9 $(pidof block_server_test_comm)
kill -9 $(pidof epoch_server)
kill -9 $(pidof user)
rm -f build/src/block_server/*.bin
rm -rf build/src/block_server/ycsb
rm -rf build/src/block_server/small_bank
rm -rf build/src/block_server/raft_*
rm -rf build/src/epoch_server/raft_*
rm -f cmake-build-debug/src/block_server/*.bin
rm -rf cmake-build-debug/src/block_server/ycsb
rm -rf cmake-build-debug/src/block_server/small_bank
rm -rf cmake-build-debug/src/block_server/raft_*
rm -rf cmake-build-debug/src/epoch_server/raft_*
cp -r cmake-build-debug/src/block_server/ycsb_bk cmake-build-debug/src/block_server/ycsb
cp -r cmake-build-debug/src/block_server/small_bank_bk cmake-build-debug/src/block_server/small_bank
cp -r build/src/block_server/ycsb_bk build/src/block_server/ycsb
cp -r build/src/block_server/small_bank_bk build/src/block_server/small_bank
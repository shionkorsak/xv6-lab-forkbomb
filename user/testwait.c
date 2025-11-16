#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  int status;
  
  printf("Testing wait_noblock system call...\n");
  
  // Test 1: No children - should return 0
  printf("Test 1: No zombie children\n");
  int ret = wait_noblock(&status);
  if(ret == 0) {
    printf("  PASS: wait_noblock returned 0 (no zombie children)\n");
  } else {
    printf("  FAIL: wait_noblock returned %d (expected 0)\n", ret);
  }
  
  // Test 2: Create a child that exits immediately
  printf("\nTest 2: Child exits immediately\n");
  pid = fork();
  if(pid < 0) {
    printf("  FAIL: fork failed\n");
    exit(1);
  }
  
  if(pid == 0) {
    // Child process
    exit(42);
  }
  
  // Parent process - wait a bit for child to exit
  sleep(1);
  
  ret = wait_noblock(&status);
  if(ret == pid) {
    printf("  PASS: wait_noblock returned child PID %d\n", ret);
    printf("  Child exit status: %d\n", status);
    if(status == 42) {
      printf("  PASS: Exit status is correct (42)\n");
    } else {
      printf("  FAIL: Exit status is %d (expected 42)\n", status);
    }
  } else {
    printf("  FAIL: wait_noblock returned %d (expected %d)\n", ret, pid);
  }
  
  // Test 3: No more zombie children after reaping
  printf("\nTest 3: No more zombie children after reaping\n");
  ret = wait_noblock(&status);
  if(ret == 0) {
    printf("  PASS: wait_noblock returned 0 (no more zombie children)\n");
  } else {
    printf("  FAIL: wait_noblock returned %d (expected 0)\n", ret);
  }
  
  printf("\nAll tests completed!\n");
  exit(0);
}

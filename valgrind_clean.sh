#!/bin/bash

# Valgrind script for minishell with suppressions
# This script runs Valgrind with the custom suppression file to hide
# readline/libtinfo "still reachable" memory that are not actual leaks.

VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=valgrind.supp ./minishell"

# Check if suppression file exists
if [ ! -f "valgrind.supp" ]; then
    echo "Error: valgrind.supp file not found in current directory"
    echo "Make sure you're running this from the minishell project root"
    exit 1
fi

# Check if minishell binary exists
if [ ! -f "./minishell" ]; then
    echo "Error: ./minishell binary not found"
    echo "Please compile the project first with 'make'"
    exit 1
fi

echo "Running Valgrind with suppressions for minishell..."
echo "Command: valgrind $VALGRIND_OPTS ./minishell $@"
echo ""

valgrind $VALGRIND_OPTS ./minishell "$@"

# Test command to check for memory leaks in bash
# echo 'echo "test1" | echo "test2"; exit' | valgrind --leak-check=full --suppressions=valgrind.supp bash 2>&1 | grep -A5 "LEAK SUMMARY"
# Check for leaks in bash when command not found
#valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes bash -c 'nonexistentcommand' 2>&1 | grep "LEAK SUMMARY" -A 5
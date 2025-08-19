#!/bin/bash

echo "=== Heredoc with Pipes Test ==="
echo

echo "1. Basic heredoc piped to grep:"
cat << EOF | grep "important"
This is an important line
This line is not significant
Another important message
Regular text here
EOF

echo
echo "2. Heredoc piped to sort:"
cat << EOF | sort
zebra
apple
banana
cherry
EOF

echo
echo "3. Heredoc piped through multiple commands:"
cat << EOF | grep "test" | wc -l
This is a test line
Another line without the word
This line has test in it
Another test line
EOF

echo
echo "4. Heredoc with variables piped to grep:"
KEYWORD="minishell"
cat << EOF | grep "$KEYWORD"
This is about bash
This is about minishell
This is about zsh
The minishell project is great
EOF

echo
echo "5. Complex pipeline:"
cat << EOF | tr '[:lower:]' '[:upper:]' | sort | head -2
hello world
goodbye world
amazing world
beautiful world
EOF

echo "=== End of Pipe Tests ==="

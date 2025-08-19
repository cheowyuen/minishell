#!/bin/bash

echo "=== Heredoc Test Script ==="
echo "This script tests various heredoc scenarios for minishell"
echo

echo "1. Basic heredoc with cat:"
cat << EOF
Hello World!
This is a basic heredoc test.
Multiple lines should work.
EOF

echo
echo "2. Heredoc with variables:"
NAME="Minishell"
cat << EOF
Welcome to $NAME
Current user: $USER
Current directory: $PWD
EOF

echo
echo "3. Heredoc with quoted delimiter (no variable expansion):"
cat << 'EOF'
This should NOT expand: $NAME
And this should NOT expand: $USER
EOF

echo
echo "4. Heredoc with grep:"
grep "test" << EOF
This is a test line
Another line without the word
This line has test in it
EOF

echo
echo "5. Heredoc with wc (word count):"
wc -l << EOF
Line 1
Line 2
Line 3
Line 4
EOF

echo
echo "6. Simple heredoc with echo-like behavior:"
cat << END
Simple heredoc test
Just one line
END

echo
echo "=== End of Heredoc Tests ==="

rsync -av --exclude-from "exclude_files.txt" user_20@node0.group20.420.ece.ualberta.ca:Labs/ .
git add *
git commit -m "$1"
git push
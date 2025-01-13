rsync -av --exclude-from "exclude_files.txt" -e "ssh -p 9020" user_20@129.128.215.200:Labs/ .
git add *
git commit -m "$1"
git push
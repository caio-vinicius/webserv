for i in {1..500}; do
  curl localhost:$1
  sleep 0.1
done

for radius in 3 5 11 25 51
do
  output_folder="dest-$radius"
  mkdir -p $output_folder
	python3 main.py -i "img" -o $output_folder -r $radius
done
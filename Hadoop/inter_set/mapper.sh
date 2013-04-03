awk -v fname=${map_input_file}  '{
	if (fname ~/inputB/){
		print $1"\tb"
	}
	else{
		print $1"\ta"
	}
}'

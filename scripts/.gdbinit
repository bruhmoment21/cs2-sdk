set confirm off
set $library = 0

define openlib
	set $dlopen = (void*(*)(char*, int)) dlopen
	set $dlclose = (int(*)(void*)) dlclose

	# Replace /path/to/lib with the built library's path.
	set $library = $dlopen("/path/to/lib", $arg0)
end

define lc
	openlib 6

	if $library == 0
		openlib 1
	else
		call $dlclose($library)
		echo Already loaded.\n
	end
end

define uc
	openlib 6
	
	if $library == 0
		echo Nothing to unload.\n
	else
		call $dlclose($library)
		call $dlclose($library)

		set $library = 0
	end
end

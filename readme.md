emcc main.cpp -o main.js -s MODULARIZE=1 -s 'EXPORT_NAME="createModule"' -s EXPORTED_FUNCTIONS='["_main"]'

emcc snake.cpp -o snake.js \
 -sEXPORTED_FUNCTIONS='["_start_game","_is_game_over","_get_score","_change_direction","_update","_get_snake_length","_get_snake_segment","_get_food"]' \
 -sEXPORTED_RUNTIME_METHODS='["ccall"]' \
 -sMODULARIZE=1 -sENVIRONMENT='web'

emcc snake.cpp -o snake.js -s MODULARIZE=1 -s 'EXPORT_NAME="createModule"' -s EXPORTED_FUNCTIONS='["_start_game","_is_game_over","_get_score","_change_direction","_update","_get_snake_length","_get_snake_segment","_get_food"]' -s EXPORTED_RUNTIME_METHODS='["ccall"]'

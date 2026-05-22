get_filename_component(OUTPUT_DIR "${OUTPUT_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

file(READ "${INPUT_FILE}" CONTENT HEX)

string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," CONTENT "${CONTENT}")

file(WRITE "${OUTPUT_FILE}"
  "#ifndef ${GUARD}\n"
  "#define ${GUARD}\n"
  "static const unsigned char ${VAR_NAME}[] = {${CONTENT}};\n"
  "static const unsigned int ${VAR_NAME}_len = sizeof(${VAR_NAME});\n"
  "#endif\n"
)

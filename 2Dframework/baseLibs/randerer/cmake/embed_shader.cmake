get_filename_component(OUTPUT_DIR "${OUTPUT_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

file(READ "${INPUT_FILE}" CONTENT)
string(REPLACE "\\" "\\\\" CONTENT "${CONTENT}")
string(REPLACE "\"" "\\\"" CONTENT "${CONTENT}")
string(REPLACE "\n" "\\n\"\n\"" CONTENT "${CONTENT}")

file(WRITE "${OUTPUT_FILE}"
  "#ifndef ${GUARD}\n"
  "#define ${GUARD}\n"
  "static const char* ${VAR_NAME} = \"${CONTENT}\";\n#endif\n"
)

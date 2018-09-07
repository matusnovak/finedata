static const char* getErrorStr(tinyxml2::XMLError err) {
    switch (err) {
        case tinyxml2::XMLError::XML_SUCCESS: return "XML_SUCCESS";
        case tinyxml2::XMLError::XML_NO_ATTRIBUTE: return "XML_NO_ATTRIBUTE";
        case tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE: return "XML_WRONG_ATTRIBUTE_TYPE";
        case tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND: return "XML_ERROR_FILE_NOT_FOUND";
        case tinyxml2::XMLError::XML_ERROR_FILE_COULD_NOT_BE_OPENED: return "XML_ERROR_FILE_COULD_NOT_BE_OPENED";
        case tinyxml2::XMLError::XML_ERROR_FILE_READ_ERROR: return "XML_ERROR_FILE_READ_ERROR";
        case tinyxml2::XMLError::XML_ERROR_PARSING_ELEMENT: return "XML_ERROR_PARSING_ELEMENT";
        case tinyxml2::XMLError::XML_ERROR_PARSING_ATTRIBUTE: return "XML_ERROR_PARSING_ATTRIBUTE";
        case tinyxml2::XMLError::XML_ERROR_PARSING_TEXT: return "XML_ERROR_PARSING_TEXT";
        case tinyxml2::XMLError::XML_ERROR_PARSING_CDATA: return "XML_ERROR_PARSING_CDATA";
        case tinyxml2::XMLError::XML_ERROR_PARSING_COMMENT: return "XML_ERROR_PARSING_COMMENT";
        case tinyxml2::XMLError::XML_ERROR_PARSING_DECLARATION: return "XML_ERROR_PARSING_DECLARATION";
        case tinyxml2::XMLError::XML_ERROR_PARSING_UNKNOWN: return "XML_ERROR_PARSING_UNKNOWN";
        case tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT: return "XML_ERROR_EMPTY_DOCUMENT";
        case tinyxml2::XMLError::XML_ERROR_MISMATCHED_ELEMENT: return "XML_ERROR_MISMATCHED_ELEMENT";
        case tinyxml2::XMLError::XML_ERROR_PARSING: return "XML_ERROR_PARSING";
        case tinyxml2::XMLError::XML_CAN_NOT_CONVERT_TEXT: return "XML_CAN_NOT_CONVERT_TEXT";
        case tinyxml2::XMLError::XML_NO_TEXT_NODE: return "XML_NO_TEXT_NODE";
        case tinyxml2::XMLError::XML_ERROR_COUNT: return "XML_ERROR_COUNT";
        default: break;
    }
    return "XML_UNKNOWN_ERROR";
}

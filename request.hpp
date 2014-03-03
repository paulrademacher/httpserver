#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

class Request {
public:
  std::string method;
  std::string uri;
  std::string http_version;
};

typedef std::shared_ptr<Request> RequestSharedPtr;

#endif // __HTTP_REQUEST_H__

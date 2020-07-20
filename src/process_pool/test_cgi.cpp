#include "process_pool.hpp"
/**
 * @brief 用于处理客户CGI请求，作为ProcessPool的模板参数
 * 
 */
class CGIConn {
  private:
    static const int BUFFR_SIZE =1024;
    static int epollfd_;
    int sockfd_;
    sockaddr_in client_address_;
    int read_index_;
    char recv_buf_[BUFFR_SIZE];
  public:
    CGIConn(){};
    ~CGIConn(){};
    void init(int epollfd,int sockfd,const sockaddr_in& client_addr){
      epollfd_=epollfd;
      sockfd_ =sockfd;
      client_address_ = client_addr;
      memset(recv_buf_,'\0',BUFFR_SIZE);
      read_index_ = 0;
    }

    void process(){
      int idx =0;
      int ret =-1;
      while (true){
        idx= read_index_;
        ret = recv(sockfd_,recv_buf_+idx,BUFFR_SIZE-1-idx,0);
        if(ret < 0){
            if(errno != EAGAIN)
              Removefd(epollfd_,sockfd_);
            
            break;
        }else if(ret == 0 ){
          Removefd(epollfd_,sockfd_);
          break;
        }else{
          read_index_ += ret;
          std::cout << "user conntent is: " << recv_buf_<<std::endl;

          for(;idx < read_index_ ;++idx){
            //遇到\r \n开始处理请求
            if(idx >=1 && recv_buf_[idx-1]=='\r' && recv_buf_[idx]=='\n')
              break;
          }

          //没有遇到字符“\r\n”则需要读取更多的客户端数据
          if(idx == read_index_)
            continue;  

          recv_buf_[idx -1] ='\0';

          char* fileName = recv_buf_;
          //判断CGI程序是否存在
          if(access(fileName,F_OK) == -1){
            Removefd(epollfd_,sockfd_);
            break;
          }

          //创建子进程来执行CGI
          ret = fork();
          if(ret == -1){
            Removefd(epollfd_,sockfd_);
            break;
          }else if(ret >0){
            Removefd(epollfd_,sockfd_);
            break;
          }else{
            //子进程将标准输出定向到sockfd_，并执行CGI
            close(STDOUT_FILENO);
            dup(sockfd_);
            execl(recv_buf_,recv_buf_,0);
            exit(0);
          }
        }
      }
      
    }
};
int CGIConn::epollfd_ =-1;

int main(int argc, char* argv[]) {
  if(argc <=2){
    std::cout << "usage " << basename(argv[0]) << " ip_address port "<<std::endl;
    return 1;
  }

  const char* ip =argv[1];
  int port = atoi(argv[2]);

  int listenfd = socket(PF_INET,SOCK_STREAM,0);
  assert(listenfd >=0);

  int ret = 0;
  struct sockaddr_in address;
  bzero(&address,sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  inet_pton(AF_INET,ip,&address.sin_addr);
  address.sin_port = htons(port);

  ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
  assert(ret != -1);

  ret = listen(listenfd,5);
  assert(ret !=-1);

  const int fd = listenfd;
  ProcessPool<CGIConn,8,1>* pool = ProcessPool< CGIConn,8,1 >::GetInstance(listenfd);
  if(pool){
    pool->Run();
    delete pool;
  }
  close(listenfd);
  return 0;
}
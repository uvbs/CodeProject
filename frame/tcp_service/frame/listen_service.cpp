#include "listen_service.h" 
#ifdef _WIN32
#include <windows.h>
#endif

int ListenService::initService(int TcpServiceCount)
{
#ifdef _LINUX64
    evthread_use_pthreads();
#elif _WIN32
    evthread_use_windows_threads();
#endif
    
    m_nMaxTcpService = TcpServiceCount;
    m_base = event_base_new();
    if(m_base == NULL){
        printf("ListenService::initService create fail, create event_base error.\n");
        return -1;
    }

    for (int i=0; i<m_nMaxTcpService; ++i)
    {
        TcpRecvService* pRecvService = new TcpRecvService();
        m_pRecvServices.push_back(pRecvService);
        int pipe_fd[2];

#ifdef _LINUX64
        if(pipe(pipe_fd)<0){
            printf("ListenService::initService fail, pipe create error.\n");
            return -1;
        }
#else
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL; //使用系统默认的安全描述符
        sa.bInheritHandle = TRUE; //一定要为TRUE，不然句柄不能被继承。
        CreatePipe((PHANDLE)&pipe_fd[0],(PHANDLE)&pipe_fd[1],&sa,0); //创建pipe内核对象,设置好hReadPipe,hWritePipe.
#endif
        
        pRecvService->initService(pipe_fd[0], pipe_fd[1], i);
        pRecvService->run();

        printf("ListenService::initService create TcpRecvService success, num:%d, readpipefd:%d, writefd:%d \n", 
            i, pipe_fd[0], pipe_fd[1]);
    }
    printf("ListenService::initService success, initService number:%d\n", TcpServiceCount);
    return 0;
}

void ListenService::startService(){

	try {
		if(m_base){
			event_base_dispatch(m_base);    
		}
		else{
			printf("ListenService::startService fail, Service not init, eventbase is NULL");
		}
	}catch (...) {
		
	}
}

void ListenService::freeService()
{
    if(m_base){
        event_base_free(m_base);
    }
}

TcpRecvService* ListenService::getRandomService(unsigned int fd)
{
    int index = fd%m_nMaxTcpService;
    return m_pRecvServices[index];
}

TcpRecvService* ListenService::getService(int nThreadIndex)
{
    int index = 0;
    if(nThreadIndex >= 0 && nThreadIndex < m_nMaxTcpService){
        index = nThreadIndex;
    }
    else{
        srand((int)time(NULL));
        index = rand()%m_nMaxTcpService;
        return m_pRecvServices[index];
    }

    return m_pRecvServices[index];
}

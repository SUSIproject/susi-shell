/*
 * Copyright (c) 2014, webvariants GmbH, http://www.webvariants.de
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 * 
 * @author: Tino Rusch (tino.rusch@webvariants.de)
 */

#ifndef __REQUESTHANDLERFACTORY__
#define __REQUESTHANDLERFACTORY__

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Util/Application.h"
#include "webstack/NotFoundRequestHandler.h"
#include "webstack/AssetsRequestHandler.h"
#include "webstack/SessionRequestHandler.h"
#include "webstack/RedirectRequestHandler.h"
#include "webstack/WebSocketRequestHandler.h"

#include "webstack/FormRequestHandler.h"

#include "apiserver/ApiServerForComponent.h"

namespace Susi {

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
protected:
    std::string _assetRoot;
    Susi::Api::ApiServerForComponent *_apiServer;
    std::shared_ptr<Susi::Sessions::SessionManagerComponent> _sessionManager;
public:
    RequestHandlerFactory(std::string assetRoot, 
                          Susi::Api::ApiServerForComponent *apiServer, 
                          std::shared_ptr<Susi::Sessions::SessionManagerComponent> sessionManager) : 
                            _assetRoot{assetRoot} , 
                            _apiServer(apiServer) ,
                            _sessionManager{sessionManager} {}
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request){
        Susi::Logger::debug("got request with URI: "+request.getURI());
        try{
            if(request.getURI().find("/assets/")==0){
            	return new SessionRequestHandler(new AssetsRequestHandler(_assetRoot), _sessionManager);
            }else if(request.getURI() == "/ws"){
            	return new SessionRequestHandler(new WebSocketRequestHandler(_apiServer), _sessionManager);
            }/*else if(request.getURI() == "/compability"){
                return new SessionRequestHandler(new CompabilityRequestHandler());
            }*/else if(request.getURI() == "/form"){
                return new SessionRequestHandler(new FormRequestHandler("./uploads/"), _sessionManager);
            }else if(request.getURI() == "/"){
                Susi::Logger::debug("instanciating SessionRequestHandler");
            	return new SessionRequestHandler(new RedirectRequestHandler(), _sessionManager);
            }
            return new SessionRequestHandler(new NotFoundRequestHandler(), _sessionManager);
        }catch(const std::exception & e){
            Susi::Logger::error(std::string("error in request handler factory: ")+e.what());
            return nullptr;
        }
    }
};

}

#endif // __REQUESTHANDLERFACTORY__
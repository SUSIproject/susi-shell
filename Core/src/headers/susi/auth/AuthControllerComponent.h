/*
 * Copyright (c) 2014, webvariants GmbH, http://www.webvariants.de
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 *
 * @author: Christian Sonderfeld (christian.sonderfeld@webvariants.de)
 */


#ifndef __AUTH_CONTROLLERCOMPONENT__
#define __AUTH_CONTROLLERCOMPONENT__

#include "susi/auth/AuthController.h"
#include "susi/world/SessionAwareComponent.h"
#include "susi/logger/easylogging++.h"

namespace Susi {
    namespace Auth {

        class ControllerComponent : public Controller, public Susi::System::SessionAwareComponent
        {
        protected:
            void handleLogin( Susi::Events::EventPtr event );
            void handleLogout( Susi::Events::EventPtr event );
            void handleIsLoggedIn( Susi::Events::EventPtr event );
            void handleGetUsername( Susi::Events::EventPtr event );

            void handleAddUser( Susi::Events::EventPtr event );
            void handleDelUser( Susi::Events::EventPtr event );
            void handleUpdateUsername( Susi::Events::EventPtr event );
            void handleUpdateAuthlevel( Susi::Events::EventPtr event );
            void handleUpdatePassword( Susi::Events::EventPtr event );

        public:
            ControllerComponent( Susi::System::ComponentManager * mgr, std::string db_identifier ) :
                Controller {
                    mgr->getComponent<Susi::DB::DBComponent>( "dbmanager" ),
                    mgr->getComponent<Susi::Sessions::SessionManagerComponent>( "sessionmanager" ),
                    db_identifier
                },
                Susi::System::SessionAwareComponent {mgr} {}

            virtual void start() override {
                subscribe( std::string{"auth::login"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    handleLogin( std::move( evt ) );

                }});
                subscribe( std::string{"auth::logout"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    handleLogout( std::move( evt ) );
                }});
                subscribe( std::string{"auth::isLoggedIn"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    LOG(INFO) << "handle auth::isLoggedIn";
                    handleIsLoggedIn( std::move( evt ) );
                }});
                subscribe( std::string{"auth::getUsername"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    handleGetUsername( std::move( evt ) );
                }});

                subscribe( std::string{"auth::addUser"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    markConfidential(*evt,0);
                    assertAuthlevel(*evt,0);
                    handleAddUser( std::move( evt ) );
                }});
                subscribe( std::string{"auth::delUser"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    markConfidential(*evt,0);
                    assertAuthlevel(*evt,0);
                    handleDelUser( std::move( evt ) );
                }});

                subscribe( std::string{"auth::updateUsername"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    markConfidential(*evt,0);
                    assertAuthlevel(*evt,0);
                    handleUpdateUsername( std::move( evt ) );
                }});
                subscribe( std::string{"auth::updateAuthlevel"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    markConfidential(*evt,0);
                    assertAuthlevel(*evt,0);
                    handleUpdateAuthlevel( std::move( evt ) );
                }});
                subscribe( std::string{"auth::updatePassword"}, Susi::Events::Processor{[this]( Susi::Events::EventPtr evt ) {
                    markConfidential(*evt,0);
                    assertAuthlevel(*evt,0);
                    handleUpdatePassword( std::move( evt ) );
                }});
                LOG(INFO) <<  "started AuthControllerComponent" ;
            }

            virtual void stop() override {
                unsubscribeAll();
            }

            ~ControllerComponent() {
                stop();
                LOG(INFO) <<  "stopped AuthControllerComponent" ;
            }
        };

    }
}

#endif // __AUTH_CONTROLLERCOMPONENT__

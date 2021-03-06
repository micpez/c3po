/*
* Copyright (c) 2017 Sprint
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __DATAACCESS_H
#define __DATAACCESS_H

#include <stdexcept>
#include <string>
#include <list>
#include <map>

#include "scassandra.h"
#include "stime.h"

#include "rule.h"
#include "apn.h"
#include "endpoint.h"
#include "session.h"
#include "bearer.h"
#include "subscriber.h"

class DAException : public std::runtime_error
{
public:
   DAException(const char *m) : std::runtime_error(m) {}
   DAException(const std::string &m) : std::runtime_error(m) {}
};

class DataAccess
{
public:
   DataAccess();
   ~DataAccess();

   std::string &host( const char *hst ) { return m_db.host(hst); }
   std::string &host( const std::string &hst ) { return m_db.host(hst); }
   std::string &host() { return m_db.host(); }

   std::string &keyspace( const char *ks ) { return m_db.keyspace(ks); }
   std::string &keyspace( const std::string &ks ) { return m_db.keyspace(ks); }
   std::string &keyspace() { return m_db.keyspace(); }

   int protocolVersion( int pv ) { return m_db.protocolVersion(pv); }
   int protocolVersion() { return m_db.protocolVersion(); }

   void connect();
   void connect( const std::string &hst, const std::string &ks = "pcrf" );
   void connect( const char *hst, const char *ks = "pcrf" );

   void disconnect();

   bool getRules( RulesMap &rules );
   bool getApns( ApnMap &apns, RulesMap &rules );
   bool getEndpoints( EndpointMap &endpoints );

   bool addEndpoint( const Endpoint &ep );

   bool addSession( GxSession &s );
   void deleteSession( GxSession &s );
   bool sessionExists( const char *imsi, const char *apn );
   bool sessionExists( const std::string &imsi, const std::string &apn ) { return sessionExists( imsi.c_str(), apn.c_str() ); }

   bool getSubscriber( const char *imsi, Subscriber &sub, RulesMap &rules );
   bool getSubscriber( const std::string &imsi, Subscriber &sub, RulesMap &rules ) { return getSubscriber( imsi.c_str(), sub, rules ); }
   bool addSubscriber( const Subscriber &s );
   bool addSubscriberApn( const char *imsi, const SubscriberApn &sa );
   bool addSubscriberApn( const std::string &imsi, const SubscriberApn &sa ) { return addSubscriberApn( imsi.c_str(), sa ); }
   void deleteSubscriber( const Subscriber &s );

private:
   void _concatenateRules( const RulesList &rules, std::string &str );

   bool _addSession( GxSession &s, const std::string &ipv4, const std::string &ipv6 );
   bool _addSessionIP( const std::string &ip, const std::string &imsi, const std::string &apn );
   bool _addSessionBearer( const std::string &imsi, const std::string &apn, Bearer &b );
   void _deleteSession( GxSession &s );
   void _deleteSessionIP( const std::string &ip, const std::string &imsi, const std::string &apn );
   void _deleteSessionBearer( const std::string &imsi, const std::string &apn, Bearer &b );
   bool _getSubscriber( const char *imsi, Subscriber &sub );
   bool _getSubscriberApns( const char *imsi, Subscriber &sub, RulesMap &rules );
   bool _addSubscriber( const Subscriber &s );
   bool _addSubscriberApn( const char *imsi, const SubscriberApn &sa );
   void _deleteSubscriber( const char *imsi );
   void _deleteSubscriberApn( const char *imsi, const char *apn );

   SCassandra m_db;
};

#endif // __DATAACCESS_H

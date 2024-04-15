#include <cassandra.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <chrono>



typedef struct Credentials_ {
  const char* password;
  const char* username;
} Credentials;

void on_auth_initial(CassAuthenticator* auth, void* data) {
  /*
   * This callback is used to initiate a request to begin an authentication
   * exchange. Required resources can be acquired and initialized here.
   *
   * Resources required for this specific exchange can be stored in the
   * auth->data field and will be available in the subsequent challenge
   * and success phases of the exchange. The cleanup callback should be used to
   * free these resources.
   */

  /*
   * The data parameter contains the credentials passed in when the
   * authentication callbacks were set and is available to all
   * authentication exchanges.
   */
  const Credentials* credentials = (const Credentials*)data;

  size_t username_size = strlen(credentials->username);
  size_t password_size = strlen(credentials->password);
  size_t size = username_size + password_size + 2;

  char* response = cass_authenticator_response(auth, size);

  /* Credentials are prefixed with '\0' */
  response[0] = '\0';
  memcpy(response + 1, credentials->username, username_size);

  response[username_size + 1] = '\0';
  memcpy(response + username_size + 2, credentials->password, password_size);
}

void on_auth_challenge(CassAuthenticator* auth, void* data, const char* token, size_t token_size) {
  /*
   * Not used for plain text authentication, but this is to be used
   * for handling an authentication challenge initiated by the server.
   */
}

void on_auth_success(CassAuthenticator* auth, void* data, const char* token, size_t token_size) {
  /*
   * Not used for plain text authentication, but this is to be used
   * for handling the success phase of an exchange.
   */
}

void on_auth_cleanup(CassAuthenticator* auth, void* data) {
  /*
   * No resources cleanup is necessary for plain text authentication, but
   * this is used to cleanup resources acquired during the authentication
   * exchange.
   */
}



int main(int argc, char* argv[]) {
    /* Setup and connect to cluster */
    // 开始计时
    auto startTime = std::chrono::high_resolution_clock::now();
    int total_req_count = 1000;

    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    char* hosts = "172.31.16.245";

    if (argc > 1) {
        total_req_count = std::stoi(argv[1]);
    }

    cass_cluster_set_credentials(cluster, "cassandra", "cassandra");
  


    /* Add contact points */
    cass_cluster_set_contact_points(cluster, hosts);


    /* Provide the cluster object as configuration to connect the session */
    connect_future = cass_session_connect(session, cluster);

    /* This will block until the query has finished */
    CassError rc = cass_future_error_code(connect_future);
    if (rc == CASS_OK) {
        printf("connect result: %s\n", cass_error_desc(rc));
        CassFuture* close_future = NULL;

        /* Build statement and execute query */
        std::string query = "SELECT emp_id,emp_name,emp_city,emp_phone,emp_sal FROM rs_test_space.emp WHERE emp_id = ";
        for(int i = 0; i < total_req_count; i++) {
            int j = (i % 11);
            std::string tmp_query = query; 
            tmp_query.append(std::to_string(j)).append(";");
            std::cout << tmp_query << std::endl;
            //可以复用么？
            CassStatement* statement = cass_statement_new(tmp_query.c_str(), 0);
            CassFuture* result_future = cass_session_execute(session, statement);

            CassError rc = cass_future_error_code(result_future);
            // printf("Query result: %s\n", cass_error_desc(rc));
            if (rc == CASS_OK) {
                /* Retrieve result set and get the first row */
                const CassResult* result = cass_future_get_result(result_future);
                const CassRow* row = cass_result_first_row(result);

                if (row) {
                      const CassValue* value = cass_row_get_column_by_name(row, "emp_city");

                      const char* release_version = NULL;
                      size_t release_version_length = 0;
                      cass_value_get_string(value, &release_version, &release_version_length);

                      const CassValue* id_value = cass_row_get_column_by_name(row, "emp_id");
                      int id = -1;
                      cass_value_get_int32(id_value, &id);


                      release_version_length = 0;
                      const char* emp_city = NULL;
                      const CassValue* emp_city_value = cass_row_get_column_by_name(row, "emp_city");
                      cass_value_get_string(emp_city_value, &emp_city, &release_version_length);



                      std::cout << "length:" << (int)release_version_length
                                << ", emp_name:" << release_version
                                << ", id:" << id
                                << ", emp_city:" << emp_city
                                << std::endl;
                  } else {
                    std::cout << "null ptr";
                  }

                cass_result_free(result);
            } else {
                /* Handle error */
                const char* message;
                size_t message_length;
                cass_future_error_message(result_future, &message, &message_length);
                fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length, message);
          }

          cass_statement_free(statement);
          cass_future_free(result_future);
      }

      /* Close the session */
      close_future = cass_session_close(session);
      cass_future_wait(close_future);
      cass_future_free(close_future);
  } else {
    /* Handle error */
    const char* message;
    size_t message_length;
    cass_future_error_message(connect_future, &message, &message_length);
    fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
  }

  cass_future_free(connect_future);
  cass_cluster_free(cluster);
  cass_session_free(session);
  // 结束计时并计算耗时
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    // 输出耗时结果
    std::cout << "cassandra driver total_req_count:" << total_req_count << ",Execution time: " << duration.count() << " microseconds" << std::endl;
 
  return 0;
}
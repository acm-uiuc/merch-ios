//
//  APIManager.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

public typealias APISuccessBlock = (_ json: Any) -> Void
public typealias APIErrorBlock = (_ error: String) -> Void

struct APIRequest {
    enum RequestType: String {
        case GET
        case POST
        case DELETE
        case PUT
    }
    
    static let baseURL = "https://api.acm.illinois.edu/merch"
    
    var endpoint = ""
    var queryParams = [String: String]()
    
    var success: APISuccessBlock?
    var error: APIErrorBlock?
    
    var requestType = RequestType.GET
    
    // URL Request
    func urlRequest(withAuthorization authorization: UserModel?) -> URLRequest {
        var queryParams = self.queryParams
        
        if let netID = authorization?.netID {
            queryParams["netID"] = netID
//            queryParams["token"] = token
        }
        
        let query = queryParams.map { return "\($0)=\($1)" }.joined(separator: "&")
        
        var urlString = APIRequest.baseURL + endpoint
        
        if query != "" {
            urlString += "?" + query
        }
        
        var request = URLRequest(url: URL(string: urlString)!)
        request.httpMethod = requestType.rawValue
        
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.addValue(API_CLIENT_KEY, forHTTPHeaderField: "Authorization")
        
        print("🚀", requestType.rawValue, urlString)
        
        return request
    }
    
    // MARK: - Endpoints
    private init(endpoint: String, queryParams: [String: String], success: APISuccessBlock?, error: APIErrorBlock?, requestType: APIRequest.RequestType) {
        self.endpoint = endpoint
        self.queryParams = queryParams
        self.success = success
        self.error = error
        self.requestType = requestType
    }
    
    static func getUser(passcode: String, success: APISuccessBlock?, error: APIErrorBlock?) -> APIRequest {
        return APIRequest(endpoint: "/users/pins/\(passcode)", queryParams: [:], success: success, error: error, requestType: .GET)
    }
    
    static func getItems(success: APISuccessBlock?, error: APIErrorBlock?) -> APIRequest {
        return APIRequest(endpoint: "/items", queryParams: [:], success: success, error: error, requestType: .GET)
    }
    
    // MARK: - Perform Request
    func perform(withAuthorization authorization: UserModel?) {
        let request = urlRequest(withAuthorization: authorization)
        
        let task = URLSession.shared.dataTask(with: request) { (data, response, error) in
            if let error = error {
                self.error?(error.localizedDescription)
                return
            }
            
            if let response = response as? HTTPURLResponse, let data = data {
                if response.statusCode != 200 {
                    self.error?(HTTPURLResponse.localizedString(forStatusCode: response.statusCode))
                    return
                }
                
                if let json = (try? JSONSerialization.jsonObject(with: data, options: .allowFragments)) as? [String: Any] {
                    if let error = json["error"] as? String {
                        self.error?(error)
                        return
                    }
                    
                    if let data = json["data"] {
                        self.success?(data)
                        return
                        
                    }
                }
            }
            
            self.error?("Internal error; unable parse returned data.")
            return
        }
        task.resume()
    }
}





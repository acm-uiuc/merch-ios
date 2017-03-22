//
//  APIManager.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

public typealias SuccessBlock = (Void) -> Void
public typealias ErrorBlock = (_ error: String) -> Void

class APIManager {
    
    
    
}

struct APIEndpoint {
    enum RequestType: String {
        case GET
        case POST
        case DELETE
        case PUT
    }
    
    static let baseURL = "http://api.acm.illinois.edu"
    
    var endpoint = ""
    var queryParams = [String: String]()
//    var data: [String: Any]?
    
    var success: SuccessBlock?
    var error: ErrorBlock?
    
    var requestType = RequestType.GET
    
    // URL Request
    func urlRequest(withAuthorization authorization: UserModel?) -> URLRequest {
        var queryParams = self.queryParams
        
        if let netID = authorization?.netID, let token = authorization?.token {
            queryParams["netID"] = netID
            queryParams["token"] = token
        }
        
        let query = queryParams.map { return "\($0)=\($1)" }.joined(separator: "&")
        
        var urlString = APIEndpoint.baseURL + endpoint
        
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
//    private init() { }
    
//    func login() -> APIEndpoint {
////        return APIEndpoint(endpoint: "", queryParams: <#T##[String : String]#>, success: <#T##SuccessBlock?##SuccessBlock?##(Void) -> Void#>, error: <#T##ErrorBlock?##ErrorBlock?##(String) -> Void#>, requestType: .GET)
//    }
    
}

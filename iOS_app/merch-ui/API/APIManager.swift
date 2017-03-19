//
//  APIManager.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

class APIManager {
    
    
    
}


struct APIEndpoint {
    enum RequestType: String {
        case GET
        case POST
        case DELETE
        case PUT
    }
    
    static let baseURL = ""
    
    var endpoint = ""
    var queryParams = [String: String]()
//    var data: JSON?
    
//    var success: SuccessBlock?
//    var error: ErrorBlock?
    
    var requestType = RequestType.GET
    
    
    // URL Request
    func urlRequest(withAuthorization authorization: RequestAuthorization?) -> URLRequest? {
        var queryParams = self.queryParams
        
        if let accessToken = authorization?.accessToken, let email = authorization?.email {
            queryParams["email"] = email
            
            // addingPercentEncoding refuses to escape +, client descided to just encode +
            queryParams["email"] = queryParams["email"]?.replacingOccurrences(of: "+", with: "%2B")
            
            queryParams["token"] = accessToken
            
        }
        
        let queryComponents = queryParams.map { (key,value) -> String in
            //            return "\(key)=\(value.addingPercentEncoding(withAllowedCharacters: .urlHostAllowed) ?? "" )"
            return "\(key)=\(value)"
        }
        
        let queryString = queryComponents.joined(separator: "&")
        
        var endpointString = APIEndpoint.baseURL + endpoint
        
        if queryString != "" {
            endpointString = endpointString + "?" + queryString
        }
        print("🚀", requestType.rawValue, endpointString)
        guard let url = URL(string: endpointString) else { return nil }
        
        var request = URLRequest(url: url)
        request.httpMethod = requestType.rawValue
        
//        if let data = data {
//            //            print(data)
//            request.httpBody = try? data.rawData()
//        }
        
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        
        return request
    }
    
    
    // MARK: - Endpoints
}

//
//  GrootService.swift
//  merch-ui
//
//  Created by Rauhul Varma on 4/22/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation
import APIManager

open class GrootService: APIService {
    /// The base URL for this Service. Endpoints in this service will be postpended to this URL segment
    open class var baseURL: String {
        return "https://api.acm.illinois.edu"
    }
    
    /// Request HTTP Headers
    open class var headers: HTTPHeaders? {
        return [
            "Content-Type": "application/json",
            "Authorization": GrootService.CLIENT_KEY
        ]
    }
    
    // TODO: Document
    open class func validate(json: JSON) -> JSONValidationResult {
        if let error = json["error"] as? String {
            return .failure(error: error)
        }
        
        if json["data"] != nil {
            return .success
        }
        
        return .failure(error: "No data nor error returned.")
    }
    
    open private(set) static var CLIENT_KEY = "nil"
    
    open class func registerClientKey(_ key: String) {
        GrootService.CLIENT_KEY = key
    }
}

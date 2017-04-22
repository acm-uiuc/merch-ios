//
//  MerchService.swift
//  merch-ui
//
//  Created by Rauhul Varma on 4/22/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation
import APIManager

open class GrootMerchService: GrootService {
    override open class var baseURL: String {
        return super.baseURL + "/merch"
    }
    
    /**
     Creates a APIRequest for getting a Merch user by pin.
     
     - parameters:
         - passcode: The user's pin.
         - success: An optional success callback, called with the response json if the request succeeds.
         - failure: An optional failure callback, called with the error description if the request fails.
     
     - returns: A APIRequest<MerchService> representing this API Request.
     */
    open class func getUser(passcode: String) -> APIRequest<GrootMerchService> {
        return APIRequest<GrootMerchService>(endpoint: "/users/pins", params: [:], body: ["pin": passcode], method: .POST)
    }
    
    /**
     Creates a APIRequest for getting the items available in Merch.
     
     - parameters:
     - success: An optional success callback, called with the response json if the request succeeds.
     - failure: An optional failure callback, called with the error description if the request fails.
     
     - returns: A APIRequest<MerchService> representing this API Request.
     */
    open class func getItems() -> APIRequest<GrootMerchService> {
        return APIRequest<GrootMerchService>(endpoint: "/items", params: [:], body: nil, method: .GET)
    }
    
    /**
     Creates a APIRequest purchasing items from Merch.
     
     - parameters:
     - items: A json dictionary of items to purchase from Merch and the .
     - success: An optional success callback, called with the response json if the request succeeds.
     - failure: An optional failure callback, called with the error description if the request fails.
     
     - requires:
     `items` must be of the form
     
     ```
     [ "items" : [0, 1, 2],
     "pin": "01234567" ]
     
     ```
     
     - returns: A APIRequest<MerchService> representing this API Request.
     */
    open class func purchase(items: JSON) -> APIRequest<GrootMerchService> {
        return APIRequest<GrootMerchService>(endpoint: "/transactions", params: [:], body: items, method: .POST)
    }
    
}

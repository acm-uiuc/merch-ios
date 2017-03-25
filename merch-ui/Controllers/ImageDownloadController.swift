//
//  ImageDownloadController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/25/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation
import UIKit

public typealias ImageDownloadSuccessBlock = (_ image: UIImage) -> Void
public typealias ImageDownloadErrorBlock = (_ error: String) -> Void


class ImageDownloadController {
    static let shared = ImageDownloadController()
    private init() { }
    
    // TODO: replace UIImage with tuple (requesting, UIImage?), can use this structure to prevent duplicate requests
    var images = NSCache<NSString, UIImage>()
    
    func downloadImage(withURL urlString: String, success: ImageDownloadSuccessBlock?, error: ImageDownloadErrorBlock?) {
        guard let url = URL(string: urlString) else {
            error?("Unable to convert urlString to URL")
            return
        }
        
        URLSession.shared.dataTask(with: url) { (data, response, err) in
            if let err = err {
                error?(err.localizedDescription)
                return
            }
            
            if let response = response as? HTTPURLResponse, let data = data {
                if response.statusCode != 200 {
                    error?(HTTPURLResponse.localizedString(forStatusCode: response.statusCode))
                    return
                }
                
                if let image = UIImage(data: data) {
                    self.images.setObject(image, forKey: urlString as NSString)
                    success?(image)
                    return
                }
            }
            
            error?("Internal error; unable parse returned data.")
            return
            
        }.resume()
    }
}

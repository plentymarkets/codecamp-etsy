<?hh //strict

namespace Etsy\Api;

use Plenty\Modules\Plugin\Libs\Contracts\LibraryCallContract;
use Plenty\Plugin\ConfigRepository;

class Client
{
    /**
     * LibraryCallContract $library
     */
	private LibraryCallContract $library;

    /**
     * ConfigRepository $config
     */
    private ConfigRepository $config;

    public function __construct(LibraryCallContract $library, ConfigRepository $config)
    {
        $this->library = $library;
        $this->config = $config;
    }	

    /**
     * Call the etsy API.
     * 
     * @param  string $method The method that should be called.
     * @param  ?array<string,mixed> $params The params that should pe used for the call. Eg. /shops/:shop_id/sections/:shop_section_id -> shop_id and shop_section_id are params.
     * @param  ?array<string,mixed> $data The data that should pe used for the post call. 
     * @return ?array<string,mixed>
     */
    public function call(
        string $method, 
        ?array<string,mixed> $params = [], 
        ?array<string,mixed> $data = [], 
    ):?array<string,mixed>
    {
    	$response = $this->library->call('EtsyIntegrationShowcasePlugin::etsy_sdk', [
            'consumerKey' => $this->config->get('EtsyIntegrationShowcasePlugin.consumerKey'),
            'consumerSecret' => $this->config->get('EtsyIntegrationShowcasePlugin.consumerSecret'),
            'accessToken' => $this->config->get('EtsyIntegrationShowcasePlugin.accessToken'),
            'accessTokenSecret' => $this->config->get('EtsyIntegrationShowcasePlugin.accessTokenSecret'),

            'method' => $method,
            'params' => $params,
            'data' => $data,
        ]);
    
        return $response;
    }
}